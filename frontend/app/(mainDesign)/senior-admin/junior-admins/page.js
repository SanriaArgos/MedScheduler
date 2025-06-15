"use client";

import { useState, useEffect } from "react";
import { useRouter } from "next/navigation";
import { formatPhoneDisplay, formatPhoneForAPI, validatePhone } from '../../../utils/phoneFormatter';

export default function SeniorAdminJuniorAdminsPage() {
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState("");
    const [success, setSuccess] = useState("");
    const router = useRouter();

    // Данные администратора
    const [adminId, setAdminId] = useState(null);

    // Список младших администраторов
    const [juniorAdmins, setJuniorAdmins] = useState([]);

    // Состояние для добавления нового администратора
    const [isAddingAdmin, setIsAddingAdmin] = useState(false);
    const [newAdminData, setNewAdminData] = useState({
        lastName: "",
        firstName: "",
        patronymic: "",
        phone: "",
        password: "",
        hospitalId: "",
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
        }

        setAdminId(userData.userId);

        // Загружаем список младших администраторов
        fetchJuniorAdmins();
    }, [router]);

    useEffect(() => {
        // Фильтрация администраторов при изменении поискового запроса
        if (searchQuery) {
            const lowercaseQuery = searchQuery.toLowerCase();
            const filtered = juniorAdmins.filter(
                admin =>
                    admin.last_name.toLowerCase().includes(lowercaseQuery) ||
                    admin.first_name.toLowerCase().includes(lowercaseQuery) ||
                    (admin.patronymic && admin.patronymic.toLowerCase().includes(lowercaseQuery)) ||
                    admin.phone.toLowerCase().includes(lowercaseQuery) ||
                    admin.hospital_name.toLowerCase().includes(lowercaseQuery)
            );
            setFilteredAdmins(filtered);
        } else {
            setFilteredAdmins(juniorAdmins);
        }
    }, [searchQuery, juniorAdmins]);

    const fetchJuniorAdmins = async () => {
        try {
            // Запрос к API для получения списка младших администраторов
            const response = await fetch('https://api.medscheduler.ru/get_junior_admins');
            const data = await response.json();

            if (response.ok && data.success) {
                setJuniorAdmins(data.junior_admins);
                setFilteredAdmins(data.junior_admins);
            } else {
                console.error("API error:", data.error);
                setError(data.error || "Ошибка при получении списка младших администраторов");
                setJuniorAdmins([]);
                setFilteredAdmins([]);
            }
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
    };

    const handleAddAdmin = async (e) => {
        e.preventDefault();
        setLoading(true);
        setError("");
        setSuccess("");

        if (!validatePhone(newAdminData.phone)) {
            setError("Введите корректный российский номер телефона для нового администратора.");
            setLoading(false);
            return;
        }
        if (!newAdminData.hospitalId) {
            setError("Необходимо выбрать больницу для нового администратора.");
            setLoading(false);
            return;
        }


        const payload = {
            ...newAdminData,
            phone: formatPhoneForAPI(newAdminData.phone),
        };

        try {
            const response = await fetch("https://api.medscheduler.ru/add_junior_admin", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify(payload),
            });

            const data = await response.json();

            if (response.ok && data.success) {
                setSuccess("Младший администратор успешно добавлен");

                // Сбрасываем форму
                setNewAdminData({
                    lastName: "",
                    firstName: "",
                    patronymic: "",
                    phone: "",
                    password: "",
                    hospitalId: "",
                });

                // Обновляем список администраторов
                fetchJuniorAdmins();

                // Скрываем форму через 2 секунды
                setTimeout(() => {
                    setIsAddingAdmin(false);
                    setSuccess("");
                }, 2000);
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
            )}

            {success && (
                <div className="bg-green-100 border border-green-400 text-green-700 px-4 py-3 rounded mb-4">
                    {success}
                </div>
            )}

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
                                        <tr key={admin.user_id}>
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
                        После создания младшего администратора вы можете прикрепить его к больнице при создании новой больницы
                        или при редактировании существующей.
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
