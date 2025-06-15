"use client";

import { useState, useEffect } from "react";
import { useRouter } from "next/navigation";
import { formatPhoneDisplay, formatPhoneForAPI, validatePhone } from '@/utils/phoneFormatter';
import Link from "next/link";

export default function PatientProfile() {
    const [userData, setUserData] = useState(null);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState(null);
    const [successMessage, setSuccessMessage] = useState(null);    const [editData, setEditData] = useState({
        lastName: "",
        firstName: "",
        patronymic: "",
        phone: "",
        currentPassword: "",
        newPassword: "",
        confirmNewPassword: "",
    });
    const router = useRouter();

    useEffect(() => {        const fetchUserData = async () => {
            setLoading(true);
            try {
                const storedUser = JSON.parse(localStorage.getItem("medSchedulerUser") || "{}");
                const response = await fetch(`https://api.medscheduler.ru/get_profile_by_id?user_id=${storedUser.userId}`, {
                    method: "GET",
                    headers: {
                        "Content-Type": "application/json",
                        "Authorization": `Bearer ${localStorage.getItem("token")}`,
                    },
                });

                if (!response.ok) {
                    throw new Error("Ошибка при получении данных профиля");
                }                const data = await response.json();
                
                if (data.success) {
                    const userDataToSet = {
                        userId: storedUser.userId,
                        lastName: data.last_name || "",
                        firstName: data.first_name || "",
                        patronymic: data.patronymic || "",
                        phone: data.phone || "",
                    };                if (userDataToSet) {
                    setUserData(userDataToSet);
                    setEditData({
                        lastName: userDataToSet.lastName || "",
                        firstName: userDataToSet.firstName || "",
                        patronymic: userDataToSet.patronymic || "",
                        phone: userDataToSet.phone || "",
                        currentPassword: "",
                        newPassword: "",
                        confirmNewPassword: "",
                    });
                } else {
                    setError("Не удалось загрузить данные пользователя");
                }
            } catch (err) {
                console.error(err);
                setError("Ошибка при загрузке данных профиля");
            } finally {
                setLoading(false);
            }
        };

        fetchUserData();
    }, []);

    const handleEditChange = (e) => {
        const { name, value } = e.target;
        setEditData((prev) => ({ ...prev, [name]: value }));
    };

    const handlePhoneChange = (e) => {
        setEditData((prev) => ({ ...prev, phone: e.target.value }));
    };

    const handleSaveChanges = async () => {
        setLoading(true);
        setError(null);
        setSuccessMessage(null);

        if (editData.phone && !validatePhone(editData.phone)) {
            setError("Введите корректный  номер телефона.");
            setLoading(false);
            return;
        }        if (editData.newPassword && editData.newPassword !== editData.confirmNewPassword) {
            setError("Новые пароли не совпадают.");
            setLoading(false);
            return;
        }

        // Проверяем, что введен текущий пароль для любых изменений
        if (!editData.currentPassword) {
            setError("Введите текущий пароль для подтверждения изменений.");
            setLoading(false);
            return;
        }const payload = {
            user_id: userData.userId,
            current_password: editData.currentPassword,
            last_name: editData.lastName,
            first_name: editData.firstName,
            patronymic: editData.patronymic,
            phone: editData.phone ? formatPhoneForAPI(editData.phone) : undefined,
        };
        if (editData.newPassword) {
            payload.new_password = editData.newPassword;
            payload.new_password_repeat = editData.newPassword;
        }

        try {
            const response = await fetch("https://api.medscheduler.ru/edit_patient_profile", {
                method: "PATCH",
                headers: {
                    "Content-Type": "application/json",
                    "Authorization": `Bearer ${localStorage.getItem("token")}`,
                },
                body: JSON.stringify(payload),
            });            if (!response.ok) {
                throw new Error("Ошибка при обновлении профиля");
            }

            setSuccessMessage("Данные профиля успешно обновлены");
            setUserData((prev) => ({ ...prev, ...editData }));

            // Обновляем в localStorage, если телефон изменился
            if (editData.phone !== userData.phone) {
                const userDataFromStorage = JSON.parse(localStorage.getItem('medSchedulerUser') || '{}');
                userDataFromStorage.phone = editData.phone;
                localStorage.setItem('medSchedulerUser', JSON.stringify(userDataFromStorage));
            }
        } catch (err) {
            console.error(err);
            setError("Ошибка при обновлении профиля");
        } finally {
            setLoading(false);
        }
    };

    if (loading) {
        return (
            <div className="min-h-[calc(80vh-theme(space.36))] flex items-center justify-center">
                <div className="text-xl text-gray-600">Загрузка профиля...</div>
            </div>
        );
    }

    return (
        <div className="max-w-3xl mx-auto py-8 px-4">
            <h1 className="text-3xl font-bold text-main2 mb-8">Мой профиль</h1>

            {error && (
                <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-4">
                    {error}
                </div>
            )}

            {successMessage && (
                <div className="bg-green-100 border border-green-400 text-green-700 px-4 py-3 rounded mb-4">
                    {successMessage}
                </div>
            )}

            <div className="bg-white shadow-md rounded-lg p-6 mb-6">
                <form onSubmit={(e) => { e.preventDefault(); handleSaveChanges(); }}>
                    <div className="grid grid-cols-1 md:grid-cols-2 gap-4 mb-4">
                        <div>
                            <label htmlFor="lastName" className="block text-sm font-medium text-gray-700 mb-1">
                                Фамилия
                            </label>
                            <input
                                type="text"
                                name="lastName"
                                id="lastName"
                                value={editData.lastName}
                                onChange={handleEditChange}
                                className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                required
                            />
                        </div>
                        <div>
                            <label htmlFor="firstName" className="block text-sm font-medium text-gray-700 mb-1">
                                Имя
                            </label>
                            <input
                                type="text"
                                name="firstName"
                                id="firstName"
                                value={editData.firstName}
                                onChange={handleEditChange}
                                className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                required
                            />
                        </div>
                        <div>
                            <label htmlFor="patronymic" className="block text-sm font-medium text-gray-700 mb-1">
                                Отчество (необязательно)
                            </label>
                            <input
                                type="text"
                                name="patronymic"
                                id="patronymic"
                                value={editData.patronymic}
                                onChange={handleEditChange}
                                className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                            />
                        </div>
                        <div>
                            <label htmlFor="phone" className="block text-sm font-medium text-gray-700 mb-1">
                                Номер телефона
                            </label>
                            <input
                                type="tel"
                                name="phone"
                                id="phone"
                                value={formatPhoneDisplay(editData.phone)}
                                onChange={handlePhoneChange}
                                className="mt-1 block w-full px-3 py-2 border border-gray-300 rounded-md shadow-sm focus:outline-none focus:ring-indigo-500 focus:border-indigo-500 sm:text-sm"
                                placeholder="+7 (999) 123-45-67"
                            />
                        </div>
                    </div>                    <div className="mt-4 mb-6">
                        <h3 className="text-lg font-medium mb-2">Изменение пароля (необязательно)</h3>
                        <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                            <div className="md:col-span-2">
                                <label htmlFor="newPassword" className="block text-sm font-medium text-gray-700 mb-1">
                                    Новый пароль
                                </label>
                                <input
                                    type="password"
                                    name="newPassword"
                                    id="newPassword"
                                    value={editData.newPassword}
                                    onChange={handleEditChange}
                                    className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                />
                            </div>
                            <div className="md:col-span-2">
                                <label htmlFor="confirmNewPassword" className="block text-sm font-medium text-gray-700 mb-1">
                                    Повторите новый пароль
                                </label>
                                <input
                                    type="password"
                                    name="confirmNewPassword"
                                    id="confirmNewPassword"
                                    value={editData.confirmNewPassword}
                                    onChange={handleEditChange}
                                    className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                />
                            </div>
                        </div>
                    </div>

                    <div className="border-t pt-4">
                        <label htmlFor="currentPassword" className="block text-sm font-medium text-gray-700 mb-1">
                            Текущий пароль (для подтверждения изменений)
                        </label>
                        <input
                            type="password"
                            name="currentPassword"
                            id="currentPassword"
                            value={editData.currentPassword}
                            onChange={handleEditChange}
                            className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                            required
                        />
                    </div>

                    <div className="flex gap-3 mt-6">
                        <button
                            type="submit"
                            className="px-4 py-2 bg-main text-white rounded hover:bg-main2 transition-colors"
                        >
                            Сохранить изменения
                        </button>
                    </div>
                </form>
            </div>

            <div className="bg-white shadow-md rounded-lg p-6">
                <h2 className="text-xl font-semibold text-main2 mb-4">Мои данные</h2>

                <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                    <Link href="/patient/appointments" className="p-4 bg-gray-50 rounded-lg hover:bg-gray-100 transition-colors">
                        <h3 className="text-lg font-semibold text-main2">Мои записи</h3>
                        <p className="text-gray-600 mt-1">Просмотр текущих записей к врачам</p>
                    </Link>

                    <Link href="/patient/waitlist" className="p-4 bg-gray-50 rounded-lg hover:bg-gray-100 transition-colors">
                        <h3 className="text-lg font-semibold text-main2">Лист ожидания</h3>
                        <p className="text-gray-600 mt-1">Просмотр позиций в листах ожидания</p>
                    </Link>
                </div>
            </div>

            <div className="flex justify-between mt-8">
                <Link href="/dashboard" className="text-main hover:text-main2 transition-colors">
                    ← Вернуться в личный кабинет
                </Link>
                <Link href="/search" className="text-main hover:text-main2 transition-colors">
                    Найти врача →
                </Link>
            </div>
        </div>
    );
}
