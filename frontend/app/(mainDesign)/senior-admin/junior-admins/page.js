"use client";

import { useState, useEffect } from "react";
import { useRouter } from "next/navigation";
import { formatPhoneDisplay, formatPhoneForAPI, validatePhone } from '@/utils/phoneFormatter';
import Link from "next/link";

export default function SeniorAdminJuniorAdminsPage() {
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState("");
    const [success, setSuccess] = useState("");
    const router = useRouter();

    // Данные администратора
    const [adminId, setAdminId] = useState(null);    // Список младших администраторов    // Список младших администраторов
    const [juniorAdmins, setJuniorAdmins] = useState([]);// Состояние для добавления нового администратора
    const [isAddingAdmin, setIsAddingAdmin] = useState(false);
    const [newAdminData, setNewAdminData] = useState({
        lastName: "",
        firstName: "",
        patronymic: "",
        phone: "",
    });

    // Поиск администраторов
    const [searchQuery, setSearchQuery] = useState("");
    const [filteredAdmins, setFilteredAdmins] = useState([]);

    useEffect(() => {
        // Проверка аутентификации
        const isLoggedIn = localStorage.getItem('isLoggedIn');
        const userData = JSON.parse(localStorage.getItem('medSchedulerUser') || '{}');

        if (!isLoggedIn || !userData.userId || userData.userType !== 'senior') {
            router.push('/login');
            return;
        }        setAdminId(userData.userId);        // Загружаем список младших администраторов
        fetchJuniorAdmins();
    }, [router]);

    useEffect(() => {
        // Фильтрация администраторов при изменении поискового запроса
        if (searchQuery) {
            const lowercaseQuery = searchQuery.toLowerCase();
            const filtered = juniorAdmins.filter(
                admin =>                    admin.last_name.toLowerCase().includes(lowercaseQuery) ||
                    admin.first_name.toLowerCase().includes(lowercaseQuery) ||
                    (admin.patronymic && admin.patronymic.toLowerCase().includes(lowercaseQuery)) ||
                    admin.phone.toLowerCase().includes(lowercaseQuery) ||
                    (admin.hospital_name && admin.hospital_name.toLowerCase().includes(lowercaseQuery))
            );
            setFilteredAdmins(filtered);
        } else {
            setFilteredAdmins(juniorAdmins);
        }
    }, [searchQuery, juniorAdmins]);    const fetchJuniorAdmins = async () => {
        try {
            // Запрос к API для получения списка всех пользователей
            const usersResponse = await fetch('https://api.medscheduler.ru/get_users');
            const usersData = await usersResponse.json();

            if (!usersResponse.ok || !usersData.success) {
                console.error("API error:", usersData.error);
                setError(usersData.error || "Ошибка при получении списка младших администраторов");
                setJuniorAdmins([]);
                setFilteredAdmins([]);
                return;
            }

            // Запрос к API для получения списка больниц
            const hospitalsResponse = await fetch('https://api.medscheduler.ru/get_hospitals');
            const hospitalsData = await hospitalsResponse.json();            let hospitalsMap = {};
            if (hospitalsResponse.ok && hospitalsData.success) {
                // Создаем карту больниц по administrator_id              
                hospitalsData.hospitals.forEach(hospital => {
                    if (hospital.administrator_id) {
                        hospitalsMap[String(hospital.administrator_id)] = hospital.full_name;
                    }
                });
            }

            // Фильтруем только младших администраторов и обогащаем данными о больницах
            const juniorAdmins = usersData.users
                .filter(user => user.user_type === 'junior administrator').map(admin => {
                    const adminId = String(admin.id);
                    const hospitalName = hospitalsMap[adminId] || null;
                    return {
                        ...admin,
                        hospital_name: hospitalName
                    };
                });

            setJuniorAdmins(juniorAdmins);
            setFilteredAdmins(juniorAdmins);
        } catch (err) {
            console.error("Error fetching junior admins:", err);
            setError("Не удалось загрузить список младших администраторов");
            setJuniorAdmins([]);
            setFilteredAdmins([]);
        } finally {
            setLoading(false);
        }
    };

    const handleInputChange = (e) => {
        const { name, value } = e.target;
        setNewAdminData((prev) => ({ ...prev, [name]: value }));
    };

    const handleNewAdminPhoneChange = (e) => {
        setNewAdminData((prev) => ({ ...prev, phone: e.target.value }));
    };    const handleAddAdmin = async (e) => {
        e.preventDefault();
        setLoading(true);
        setError("");
        setSuccess("");

        if (!validatePhone(newAdminData.phone)) {
            setError("Введите корректный номер телефона для нового администратора.");
            setLoading(false);
            return;
        }

        const payload = {
            last_name: newAdminData.lastName,
            first_name: newAdminData.firstName,
            patronymic: newAdminData.patronymic,
            phone: formatPhoneForAPI(newAdminData.phone),
            //hospital_id: 0 // ТАК НАДО, СДЕЛАНО ЧЕРЕЗ 0, чтобы не привязывать к больнице
            // Пароль по умолчанию не передаем, он будет установлен на сервере
        };

        try {
            const response = await fetch("https://api.medscheduler.ru/add_junior_admin", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify(payload),
            });

            const data = await response.json();

            if (response.ok && data.success) {
                setSuccess("Младший администратор успешно добавлен");                // Сбрасываем форму
                setNewAdminData({
                    lastName: "",
                    firstName: "",
                    patronymic: "",
                    phone: "",
                });

                // Обновляем список администраторов
                fetchJuniorAdmins();

                // Скрываем форму через 2 секунды
                setTimeout(() => {
                    setIsAddingAdmin(false);
                    setSuccess("");
                }, 10000);
            } else {
                setError(data.error || "Ошибка при добавлении администратора");
            }
        } catch (err) {
            console.error("Error adding junior admin:", err);
            setError("Не удалось подключиться к серверу");
        } finally {
            setLoading(false);
        }
    };

    if (loading) {
        return (
            <div className="min-h-[calc(80vh-theme(space.36))] flex items-center justify-center">
                <div className="text-xl text-gray-600">Загрузка данных...</div>
            </div>
        );
    }

    return (
        <div className="max-w-6xl mx-auto py-8 px-4">
            <h1 className="text-3xl font-bold text-main2 mb-8">Управление младшими администраторами</h1>

            {error && (
                <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-4">
                    {error}
                </div>
            )}            {success && (
                <div className="bg-green-100 border border-green-400 text-green-700 px-4 py-3 rounded mb-4">
                    {success}
                </div>
            )}

            {/* DEBUG INFO */}
            <div className="hidden bg-yellow-50 border border-yellow-200 text-yellow-800 px-4 py-3 rounded mb-4">
                <h3 className="font-bold">Debug Info:</h3>
                <p>Total admins: {juniorAdmins.length}</p>
                <p>Admins with hospitals: {juniorAdmins.filter(a => a.hospital_name).length}</p>
                <div className="mt-2">
                    {juniorAdmins.slice(0, 3).map(admin => (
                        <div key={admin.id}>
                            ID: {admin.id}, Name: {admin.first_name} {admin.last_name}, Hospital: {admin.hospital_name || 'null'}
                        </div>
                    ))}
                </div>
            </div>

            <div className="bg-white shadow-md rounded-lg p-6 mb-6">
                <div className="flex flex-col md:flex-row justify-between md:items-center mb-6">
                    <h2 className="text-xl font-semibold text-main2">Список младших администраторов</h2>
                    <div className="mt-4 md:mt-0">
                        <button
                            onClick={() => setIsAddingAdmin(true)}
                            className="px-4 py-2 bg-main text-white rounded hover:bg-main2 transition-colors"
                        >
                            Добавить администратора
                        </button>
                    </div>
                </div>

                {/* Поиск администраторов */}
                <div className="mb-6">
                    <label htmlFor="search" className="block text-sm font-medium text-gray-700 mb-1">
                        Поиск администраторов
                    </label>
                    <input
                        type="text"
                        id="search"
                        value={searchQuery}
                        onChange={(e) => setSearchQuery(e.target.value)}
                        placeholder="Поиск по ФИО, телефону или названию больницы"
                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                    />
                </div>

                {isAddingAdmin && (
                    <div className="mb-6 p-4 bg-gray-50 rounded-lg">
                        <h3 className="text-lg font-medium text-gray-800 mb-4">Добавить нового младшего администратора</h3>
                        <form onSubmit={handleAddAdmin}>
                            <div className="grid grid-cols-1 md:grid-cols-2 gap-4 mb-4">
                                <div>
                                    <label htmlFor="lastName" className="block text-sm font-medium text-gray-700 mb-1">
                                        Фамилия *
                                    </label>
                                    <input
                                        type="text"
                                        id="lastName"
                                        name="lastName"
                                        value={newAdminData.lastName}
                                        onChange={handleInputChange}
                                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                        required
                                    />
                                </div>
                                <div>
                                    <label htmlFor="firstName" className="block text-sm font-medium text-gray-700 mb-1">
                                        Имя *
                                    </label>
                                    <input
                                        type="text"
                                        id="firstName"
                                        name="firstName"
                                        value={newAdminData.firstName}
                                        onChange={handleInputChange}
                                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                        required
                                    />
                                </div>
                                <div>
                                    <label htmlFor="patronymic" className="block text-sm font-medium text-gray-700 mb-1">
                                        Отчество
                                    </label>
                                    <input
                                        type="text"
                                        id="patronymic"
                                        name="patronymic"
                                        value={newAdminData.patronymic}
                                        onChange={handleInputChange}
                                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                    />
                                </div>
                                <div>
                                    <label htmlFor="phone" className="block text-sm font-medium text-gray-700 mb-1">
                                        Телефон * (11 цифр)
                                    </label>
                                    <input
                                        type="tel"
                                        name="phone"
                                        value={formatPhoneDisplay(newAdminData.phone)}
                                        onChange={handleNewAdminPhoneChange}
                                        className="w-full px-3 py-2 border border-gray-300 rounded-md"
                                        placeholder="+7 (999) 123-45-67"
                                        required
                                    />
                                </div>
                            </div>

                            <div className="mb-3">
                                <p className="text-sm text-gray-500">
                                    * Пароль по умолчанию: <span className="font-medium">0987654321</span>
                                </p>
                                <p className="text-sm text-gray-500">
                                    Администратор сможет изменить пароль после первого входа в систему.
                                </p>
                            </div>

                            <div className="flex gap-3">
                                <button
                                    type="submit"
                                    className="px-4 py-2 bg-main text-white rounded hover:bg-main2 transition-colors"
                                >
                                    Добавить администратора
                                </button>
                                <button
                                    type="button"
                                    onClick={() => setIsAddingAdmin(false)}
                                    className="px-4 py-2 bg-gray-200 text-gray-800 rounded hover:bg-gray-300 transition-colors"
                                >
                                    Отмена
                                </button>
                            </div>
                        </form>
                    </div>
                )}

                {/* Список администраторов */}
                <div>
                    {filteredAdmins.length === 0 ? (
                        <p className="text-gray-500 text-center py-4">Администраторы не найдены</p>
                    ) : (
                        <div className="overflow-x-auto">
                            <table className="min-w-full divide-y divide-gray-200">
                                <thead className="bg-gray-50">
                                    <tr>
                                        <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                            ФИО
                                        </th>
                                        <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                            Телефон
                                        </th>
                                        <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                            Больница
                                        </th>
                                    </tr>
                                </thead>
                                <tbody className="bg-white divide-y divide-gray-200">
                                    {filteredAdmins.map((admin) => (
                                        <tr key={admin.id}>
                                            <td className="px-6 py-4 whitespace-nowrap">
                                                <div className="text-sm font-medium text-gray-900">
                                                    {admin.last_name} {admin.first_name} {admin.patronymic || ""}
                                                </div>
                                            </td>
                                            <td className="px-6 py-4 whitespace-nowrap">
                                                <div className="text-sm text-gray-500">{admin.phone}</div>
                                            </td>
                                            <td className="px-6 py-4 whitespace-nowrap">
                                                <div className="text-sm text-gray-500">
                                                    {admin.hospital_name || "Не привязан к больнице"}
                                                </div>
                                            </td>
                                        </tr>
                                    ))}
                                </tbody>
                            </table>
                        </div>
                    )}
                </div>
            </div>

            <div className="bg-white shadow-md rounded-lg p-6 mb-6">
                <h2 className="text-xl font-semibold text-main2 mb-4">О младших администраторах</h2>
                <div className="prose prose-sm max-w-none text-gray-600">
                    <p>
                        Младшие администраторы отвечают за управление больницами, к которым они прикреплены.
                        Они могут добавлять врачей, прикреплять их к больнице, управлять расписанием и следить за листом ожидания.
                    </p>
                    <p>
                        <strong>Важно:</strong> Вновь созданные администраторы изначально не привязаны к больнице. 
                        Привязка к больнице происходит при создании новой больницы или редактировании существующей.
                    </p>
                    <p>
                        Процесс создания первой больницы: сначала создайте младшего администратора здесь, 
                        затем перейдите в раздел "Управление больницами" и создайте больницу, указав этого администратора.
                    </p>
                </div>
            </div>

            <div className="flex justify-between">
                <Link href="/senior-admin/hospitals" className="text-main hover:text-main2 transition-colors">
                    ← Управление больницами
                </Link>
                <Link href="/senior-admin/users" className="text-main hover:text-main2 transition-colors">
                    Управление пользователями →
                </Link>
            </div>
        </div>
    );
}
